class SRV_RulesUI: UIScriptedMenu
{
	const string m_LayoutPath = "ServerRulesViewer/ServerRulesViewer/GUI/layouts/SRV_RulesUI.layout";
	protected GridSpacerWidget m_CategoriesGrid;

	protected ButtonWidget m_ExitBtn;
	protected ButtonWidget m_DonateBtn;
	
	protected TextWidget m_TitleTW;
	protected TextWidget m_ExitBtnTW;
	
	protected ImageWidget m_DonateIW;
	
	protected Widget m_RuleContentPW;
	
	protected ref array<ref SRV_CategoryWidget> m_CategoryWidgets;
	
	protected ref SRV_Config m_Config;
	
	const int m_CategoriesPerPage = 8;
	
	protected int m_CategoriesPageIndex;
	protected int m_CategoriesMaxPages;
	
	protected const string m_IconHeartRed = "set:srv_ui image:srv_heart_red";
	protected const string m_IconHeartWhite = "set:srv_ui image:srv_heart_white";
	
	protected SRV_CategoryWidget m_SelectedCategory;
	
	override Widget Init()
	{
		m_Config 				= GetSRVConfigManagerPlugin().GetConfig();
        layoutRoot	 			= GetGame().GetWorkspace().CreateWidgets(m_LayoutPath);
		
		m_CategoriesGrid 		= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("categoriesGrid"));
		
        m_ExitBtn 				= ButtonWidget.Cast(layoutRoot.FindAnyWidget("exitBtn"));
        m_DonateBtn 			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("donateBtn"));
		
        m_TitleTW 				= TextWidget.Cast(layoutRoot.FindAnyWidget("titleTW"));
        m_ExitBtnTW 			= TextWidget.Cast(layoutRoot.FindAnyWidget("exitBtnTW"));
		
		m_DonateIW				= ImageWidget.Cast(layoutRoot.FindAnyWidget("donateBtnIW"));
		
		m_RuleContentPW 		= Widget.Cast(layoutRoot.FindAnyWidget("ruleContentPW"));
		
		m_CategoryWidgets 		= new array<ref SRV_CategoryWidget>();

		m_CategoriesMaxPages 	= Math.Floor(m_Config.categories.Count() / m_CategoriesPerPage);
		
		m_DonateIW.LoadImageFile(0, m_IconHeartWhite);
		m_TitleTW.SetText(m_Config.title);
		m_RuleContentPW.Show(false);
		
		DisplayCategories(m_Config.categories);

		Print("Init SRV_RulesUI");
				
		return layoutRoot;
	}
	
	void ~SRV_RulesUI()
    {     
		m_CategoryWidgets.Clear();  
    }

	override void OnShow()
    {
        super.OnShow();
        GetGame().GetUIManager().ShowCursor(true);
        GetGame().GetMission().GetHud().Show(false);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        PPEffects.SetBlurMenu(0.3);
    }

    override void OnHide()
    {
        super.OnHide();
		GetGame().GetMission().PlayerControlEnable(true);
        GetGame().GetUIManager().ShowCursor(false);
        GetGame().GetMission().GetHud().Show(true);
        GetGame().GetUIManager().Back();
        PPEffects.SetBlurMenu(0);
    }
	
	override void Update( float timeslice )
	{
		super.Update(timeslice);
		
		if (GetGame() && GetGame().GetInput() && GetGame().GetInput().LocalPress("UAUIBack", false))
		{
			OnHide();
		}
		
		int posx, posy;
		GetMousePos(posx, posy);
		
		if (SRV_Util.IsInsideWidget(m_ExitBtn, posx, posy))
		{
			m_ExitBtnTW.SetColor(COLOR_RED);
		}
		else
		{
			m_ExitBtnTW.SetColor(COLOR_WHITE);
		}
		
		if (SRV_Util.IsInsideWidget(m_DonateBtn, posx, posy))
		{
			m_DonateIW.LoadImageFile(0, m_IconHeartRed);
		}
		else
		{
			m_DonateIW.LoadImageFile(0, m_IconHeartWhite);
		}
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (SRV_Util.IsInsideWidget(m_CategoriesGrid, x, y))
		{
			if (wheel == 1)
			{
				m_CategoriesPageIndex = Math.Clamp(--m_CategoriesPageIndex, 0, m_CategoriesMaxPages);
			}
					
			if (wheel == -1)
			{
				m_CategoriesPageIndex = Math.Clamp(++m_CategoriesPageIndex, 0, m_CategoriesMaxPages);
			}
		}
		
		return super.OnMouseWheel(w, x, y, wheel);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{		
		if (w == m_ExitBtn) 
		{
			Print("[ServerRulesViewer] m_ExitBtn clicked");
			OnHide();
			
			return true;
		}
		
		if (w == m_DonateBtn) 
		{
			Print("[ServerRulesViewer] m_DonateBtn clicked");
			
			if (m_Config.donateLink != "")
			{
				Print(m_Config.donateLink);
				GetGame().OpenURL(m_Config.donateLink);
			}
			
			return true;
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	void DisplayCategories(array<ref SRV_Category> categories)
	{
		int startIndex = m_CategoriesPageIndex * m_CategoriesPerPage;
		int endIndex = Math.Min(startIndex + m_CategoriesPerPage, m_Config.categories.Count());

		m_CategoryWidgets.Clear();
		for (int i = startIndex; i < endIndex; i++)
		{
			SRV_Category category = categories[i];
			m_CategoryWidgets.Insert(new SRV_CategoryWidget(m_CategoriesGrid, this, category));
		}
	}
	
	void OnClickCategory(SRV_CategoryWidget category)
	{	
		if (m_SelectedCategory)
			m_SelectedCategory.ColorBackground(!m_SelectedCategory.IsBackgroundColored());
		
		if (category == m_SelectedCategory)
		{
			m_SelectedCategory = null;
			m_RuleContentPW.Show(false);
		}
		else
		{
			m_SelectedCategory = category;
			m_RuleContentPW.Show(true);
		}
	}
}

class SRV_CategoryWidget extends ScriptedWidgetEventHandler 
{	
	const string m_LayoutPath = "ServerRulesViewer/ServerRulesViewer/GUI/layouts/SRV_Category.layout";
	protected SRV_RulesUI m_ParentUI;
	protected SRV_Category m_Category;
	
	protected Widget m_Root;
	protected GridSpacerWidget m_ParentWidget;
	protected ButtonWidget m_CategoryBtn;
	protected Widget m_CategoryBtnBgPW;
	protected TextWidget m_CategoryBtnTW;
	
	void SRV_CategoryWidget(GridSpacerWidget parentWidget, SRV_RulesUI ui, SRV_Category category)
	{		
		m_ParentWidget 		 		= GridSpacerWidget.Cast(parentWidget);
		m_ParentUI 			 		= SRV_RulesUI.Cast(ui);
		m_Category			 		= category;
		
		m_Root				 		= GetGame().GetWorkspace().CreateWidgets(m_LayoutPath, m_ParentWidget);
		m_CategoryBtn				= ButtonWidget.Cast(m_Root.FindAnyWidget("categoryBtn"));
		m_CategoryBtnBgPW			= Widget.Cast(m_Root.FindAnyWidget("categoryBtnBgPW"));
		m_CategoryBtnTW				= TextWidget.Cast(m_Root.FindAnyWidget("categoryBtnTW"));
		
		m_CategoryBtnTW.SetText(m_Category.category);
		
		m_Root.SetHandler(this);
		m_Root.Show(true);
	}
		
	void ~SRV_CategoryWidget()
	{
		Print("SRV_CategoryWidget deleted");
		m_ParentWidget.RemoveChild(m_Root);
	}
	
	bool IsBackgroundColored()
	{
		return m_CategoryBtnBgPW.GetAlpha() != 0;
	}
	
	void ColorBackground(bool value)
	{
		if (value)
			m_CategoryBtnBgPW.SetColor(COLOR_RED);
		else
			m_CategoryBtnBgPW.SetAlpha(0);
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		ColorBackground(true);
		m_ParentUI.OnClickCategory(this);
		
		return super.OnClick(w, x, y, button);
	}
	
	/*
	override bool OnUpdate(Widget w)
	{
		int posx, posy;
		GetMousePos(posx, posy);
		
		ColorBackground(SRV_Util.IsInsideWidget(m_CategoryBtn, posx, posy));
		
		return super.OnUpdate(w);
	}
	*/
}